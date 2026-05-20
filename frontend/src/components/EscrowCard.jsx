import { useState } from 'react';
import { depositEscrow, shipEscrow, approveEscrow } from '../api/escrow.api';
import { useAuth } from '../context/AuthContext';
import styles from './EscrowCard.module.css';

export default function EscrowCard({ escrow, onStatusChange }) {
  const { token } = useAuth();
  const [loading, setLoading] = useState(false);
  const [message, setMessage] = useState(null);
  const [error, setError] = useState(null);

  const handleDeposit = async () => {
    setLoading(true);
    setMessage(null);
    setError(null);
    const result = await depositEscrow(token, escrow.escrow_id, escrow.amount);
    if (result.success) {
      setMessage(result.message || 'Deposit successful!');
      if (onStatusChange) onStatusChange(escrow.escrow_id, 'SECURED');
    } else {
      setError(result.error);
    }
    setLoading(false);
  };

  const handleShip = async () => {
    setLoading(true);
    setMessage(null);
    setError(null);
    const result = await shipEscrow(token, escrow.escrow_id);
    if (result.success) {
      setMessage(result.message || 'Goods shipped!');
      if (onStatusChange) onStatusChange(escrow.escrow_id, 'IN_INSPECTION');
    } else {
      setError(result.error);
    }
    setLoading(false);
  };

  const handleApprove = async () => {
    setLoading(true);
    setMessage(null);
    setError(null);
    const result = await approveEscrow(token, escrow.escrow_id);
    if (result.success) {
      setMessage(result.message || 'Escrow closed!');
      if (onStatusChange) onStatusChange(escrow.escrow_id, 'CLOSED');
    } else {
      setError(result.error);
    }
    setLoading(false);
  };

  const canDeposit = escrow.status === 'INITIATED';
  const canShip = escrow.status === 'SECURED';
  const canApprove = escrow.status === 'IN_INSPECTION';

  const statusClass = styles[`status-${escrow.status?.toLowerCase()}`] || '';

  return (
    <div className={styles.card}>
      <div className={styles.header}>
        <div>
          <h3 className={styles.title}>
            Escrow #{escrow.escrow_id}
          </h3>
          <div className={`${styles.status} ${statusClass}`}>
            {escrow.status}
          </div>
        </div>
        <div className={styles.amount}>
          <span className={styles.label}>Amount</span>
          <span className={styles.value}>Ksh{escrow.amount.toFixed(2)}</span>
        </div>
      </div>

      <div className={styles.details}>
        <div className={styles.detail}>
          <span className={styles.label}>Buyer</span>
          <span className={styles.value}>{escrow.buyer_username || escrow.buyer_id}</span>
        </div>
        <div className={styles.detail}>
          <span className={styles.label}>Seller</span>
          <span className={styles.value}>{escrow.seller_username || escrow.seller_id}</span>
        </div>
      </div>

      {message && (
        <div className={styles.successAlert}>
          ✓ {message}
        </div>
      )}

      {error && (
        <div className={styles.errorAlert}>
          ✗ {error}
        </div>
      )}

      <div className={styles.actions}>
        <button
          onClick={handleDeposit}
          disabled={!canDeposit || loading}
          className={`${styles.btn} ${styles.btnSuccess}`}
          title={!canDeposit ? `Status must be INITIATED (current: ${escrow.status})` : ''}
        >
          {loading ? 'Processing...' : 'Deposit Money'}
        </button>
        <button
          onClick={handleShip}
          disabled={!canShip || loading}
          className={`${styles.btn} ${styles.btnWarning}`}
          title={!canShip ? `Status must be SECURED (current: ${escrow.status})` : ''}
        >
          {loading ? 'Processing...' : 'Ship Goods'}
        </button>
        <button
          onClick={handleApprove}
          disabled={!canApprove || loading}
          className={`${styles.btn} ${styles.btnPrimary}`}
          title={!canApprove ? `Status must be IN_INSPECTION (current: ${escrow.status})` : ''}
        >
          {loading ? 'Processing...' : 'Approve Release'}
        </button>
      </div>
    </div>
  );
}
